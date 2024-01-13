#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <future>
#include <sstream>
#include "io.h"
#include "states.h"
#include "grid.h"
#include "constants.h"
#include "api.h"
#include "score.h"

#ifdef _WIN32
#include <conio.h>
bool checkIfCancelUntilStop(std::atomic<bool>& stopChecking)
{
    while (!stopChecking)
    {        
        if (_kbhit()) {
            char input = _getch();
            _putch(input);
            if (input == 'c')
            {
                clearInputBuffer();
                return true;
            }
            else
            {
                clearInputBuffer();
                printErrorMessage();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    return false;
}
#else
bool checkIfCancelUntilStop(std::atomic<bool>& stopChecking)
{
    while (!stopChecking)
    {        
        if (std::cin.rdbuf()->in_avail() > 0)
        {
            char input{};
            std::cin.get(input);
            if (input == 'c')
            {
                clearInputBuffer();
                return true;
            }
            else
            {
                invalidInput();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    return false;
}
#endif

PlayAgainStatus doPollPlayAgainStatus(
    httplib::SSLClient& client, const std::string& gameId)
{
    while (true)
    {
        try
        {
            PlayAgainStatus status{pollPlayAgainStatus(client, gameId)};
            if (status.status != PlayAgainStatus::Status::waiting)
            {
                return status;
            }
            std::this_thread::sleep_for(
                std::chrono::milliseconds(pollingRateMilisec));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void doCheckIfCancelPlayAgain(httplib::SSLClient& client,
    const std::string& gameId, std::atomic<bool>& gotPlayAgainResponse)
{
    std::cout << "Enter c to cancel play again" << std::endl;
    if (checkIfCancelUntilStop(gotPlayAgainResponse))
    {
        try
        {
            declinePlayAgain(client, gameId);
            std::cout << "Trying to decline play again" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void updateValuesWhenPlayAgainAccepted(const PlayAgainStatus& status,
    std::string& gameId, States& playerToken)
{
    if (status.switchSides)
    {
        playerToken = (playerToken == States::x) ? States::o : States::x;
    }
    gameId = status.nextGameId;
}

bool playAgain(httplib::SSLClient& client,
    std::string& gameId, States& playerToken)
{
    if (askIfPlayAgain())
    {
        try
        {
            PlayAgainStatus status{acceptPlayAgain(client, gameId)};
            if (status.status == PlayAgainStatus::Status::accepted)
            {
                updateValuesWhenPlayAgainAccepted(status, gameId, playerToken);
                std::cout << "Play again accepted" << std::endl;
                return true;
            }
            else if (status.status == PlayAgainStatus::Status::declined)
            {
                std::cout << "Play again declined" << std::endl;
                return false;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
        
        std::cout << "Waiting for opponent response" << std::endl;

        std::atomic<bool> gotPlayAgainResponse{false};
        std::thread checkIfCancelPlayAgain(doCheckIfCancelPlayAgain,
                                           std::ref(client), std::ref(gameId),
                                           std::ref(gotPlayAgainResponse));

        PlayAgainStatus status{doPollPlayAgainStatus(client, gameId)};
        gotPlayAgainResponse = true;
        checkIfCancelPlayAgain.join();

        if(status.status == PlayAgainStatus::Status::accepted)
        {
            updateValuesWhenPlayAgainAccepted(status, gameId, playerToken);
            std::cout << "Play again accepted" << std::endl;
            return true;
        }
        else
        {
            std::cout << "Play again declined" << std::endl;
            return false;
        }
    }
    else
    {
        try
        {
            declinePlayAgain(client, gameId);
            std::cout << "Play again declined" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return false;
    }
}

void playerMove(httplib::SSLClient& client,
    const std::string& gameId, Grid& grid, States side)
{
    while (true)
    {
        try
        {
            std::string cell{getCellFromUser()};
            std::stringstream cellStream{cell};
            char row{};
            int column;
            cellStream >> row >> column;
            row = std::tolower(row);
            if (grid.isEmptyCell(row, column))
            {
                makeMove(client, gameId, cell);
                grid.setCell(row, column, side);
                printMove("Your", row, column);
                return;
            }
            std::cout << "This is not a valid move" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void opponentMove(httplib::SSLClient& client,
    const std::string& gameId, Grid& grid, States side)
{
    while (true)
    {
        try
        {
            std::string newMove{pollGame(client, gameId)};
            if (newMove == "")
            {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(pollingRateMilisec));
            }
            else
            {
                std::stringstream cellStream{newMove};
                char row{};
                int column;
                cellStream >> row >> column;
                row = std::tolower(row);
                if (grid.isEmptyCell(row, column))
                {
                    grid.setCell(row, column, side);
                    printMove("Opponent's", row, column);
                    return;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void playOnline(httplib::SSLClient& client, const std::string& gameId,
    Grid& grid, States userToken, Score& score)
{
    for (int i{};; ++i)
    {
        if ((i % 2) == static_cast<int>(userToken))
        {
            playerMove(client, gameId, grid, userToken);
            std::cout << grid << '\n';
            if (grid.hasWon(userToken))
            {
                std::cout << "You win!" << std::endl;
                score.win();
                return;
            }
        }
        else
        {
            std::cout << "Waiting for opponent to move" << std::endl;
            opponentMove(client, gameId, grid,
                (userToken == States::x ? States::o : States::x));
            std::cout << grid << '\n';
            if (grid.hasWon(userToken == States::x ? States::o : States::x))
            {
                std::cout << "You lose" << std::endl;
                score.lose();
                return;
            }
        }
        if (grid.isFull())
        {
            std::cout << "That's a draw" << std::endl;
            score.draw();
            return;
        }
    }
}

void startOnline(httplib::SSLClient& client, std::string gameId,
    int gridSize, int winningLine, States playerToken)
{
    Grid grid{gridSize, winningLine};
    Score score;
    do
    {
        std::cout << grid << std::endl;
        playOnline(client, gameId, grid, playerToken, score);
        grid.clear();
        std::cout << score << std::endl;
    }
    while (playAgain(client, gameId, playerToken));
}

std::string askForInvitedName(std::vector<std::string>& waitingUsers,
    std::mutex& waitingUsersMutex)
{
    while (true)
    {
        std::string userInput{getString(
            "Enter the name of ther user you want to invite or e to exit")};
        {
            std::lock_guard<std::mutex> lock(waitingUsersMutex);
            std::atomic_thread_fence(std::memory_order_acquire);
            auto it{std::find(
                waitingUsers.begin(), waitingUsers.end(), userInput)};
            if ((it != waitingUsers.end()) || (userInput == "e"))
            {
                return userInput;
            }
            else
            {
                std::cout << userInput << 
                    " is not in the list of waiting users" << std::endl;
            }
        }
    }
}

void printWaitingUsers(const std::vector<std::string>& waitingUsers)
{
    if (waitingUsers.empty())
    {
        std::cout << "No users waiting at the moment\n";
    }
    else
    {
        std::cout << "The following users are waiting for invitation:\n";
        for (auto& user: waitingUsers)
        {
            std::cout << user << '\n';
        }
    }
}

void pollWaitingUsers(httplib::SSLClient& client,
    std::atomic<bool>& gotUserInput, std::vector<std::string>& waitingUsers,
    std::mutex& waitingUsersMutex)
{
    while (!gotUserInput)
    {
        try
        {
            std::vector<std::string> waitingUsersLatest{
                getWaitingUsers(client)};
            if (waitingUsers != waitingUsersLatest)
            {
                std::lock_guard<std::mutex> lock(waitingUsersMutex);
                waitingUsers = waitingUsersLatest;
                if (gotUserInput) return;
                std::cout << "The list of waiting users was updated\n";
                printWaitingUsers(waitingUsers);
                std::cout << "Enter the name of ther user you want to invite "
                    << "or e to exit" << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "Could not update the list of waiting users"
                << std::endl;
        }
        
        std::this_thread::sleep_for(
            std::chrono::milliseconds(pollingRateMilisec));
    }
}

InivtationStatus doPollInvitationStatus(httplib::SSLClient& client,
    const std::string& invitationId)
{
    while (true)
    {
        InivtationStatus currentStatus{pollInvitationStatus(
            client, invitationId)};
        if (currentStatus.status != "pending")
        {
            return currentStatus;
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(pollingRateMilisec));
    }
}

void doCheckIfCancelInvitation(httplib::SSLClient& client,
    const std::string& invitationId, std::atomic<bool>& gotInvitationResponse)
{
    std::cout << "Enter c to cancel invitation" << std::endl;
    if (checkIfCancelUntilStop(gotInvitationResponse))
    {
        try
        {
            cancelInvitation(client, invitationId);
            std::cout << "Trying to cancel invitation" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void invite(httplib::SSLClient& client)
{
    std::vector<std::string> waitingUsers{getWaitingUsers(client)};
    printWaitingUsers(waitingUsers);

    std::atomic<bool> gotUserInput{false};
    std::mutex waitingUsersMutex;
    std::thread pollServer(pollWaitingUsers, std::ref(client),
                           std::ref(gotUserInput), std::ref(waitingUsers),
                           std::ref(waitingUsersMutex));
    
    std::string invited{askForInvitedName(waitingUsers, waitingUsersMutex)};
    gotUserInput = true;
    pollServer.detach();

    if (invited == "e") return;

    auto gridProperties{getGridProperties()};
    States inviterToken{askForPlayerToken()};
    bool playingX{inviterToken == States::x};
    std::string playAgainScheme{playAgainSchemeToString(
        askForPlayAgainScheme())};

    std::string invitationId{sendInvitation(
        client, invited, gridProperties, playingX, playAgainScheme)};

    std::cout << "Invitation sent, waiting for opponent to respond"
        << std::endl;
    
    std::atomic<bool> gotInvitationResponse{false};
    std::thread checkIfCancel(doCheckIfCancelInvitation, std::ref(client),
                              std::ref(invitationId),
                              std::ref(gotInvitationResponse));
    
    InivtationStatus status{doPollInvitationStatus(client, invitationId)};
    gotInvitationResponse = true;
    checkIfCancel.join();

    if (status.status == "accepted")
    {
        std::cout << "Invitation accepted" << std::endl;
        startOnline(client, status.gameId, gridProperties.first,
            gridProperties.second, inviterToken);
    }
    else if (status.status == "declined")
    {
        std::cout << "Invitation declined" << std::endl;
    }
    else if(status.status == "cancelled")
    {
        std::cout << "Invitation cancelled" << std::endl;
    }
    else
    {
        std::cerr << "Unknown invitation status" << std::endl;
    }
}

std::ostream& operator<<(std::ostream& output, const Invitation& invitation)
{
    output << "from " << invitation.inviter << " (grid size: " <<
        invitation.gridSize << ", winning line: " <<
        invitation.winningLine << ", you are playing " <<
        (invitation.inviterPlayingX ? 'O' : 'X') <<
        ", play agin scheme: " << invitation.playAgainScheme << ')';

    return output;
}

void printInvitations(const std::deque<Invitation>& invitations)
{
    if (invitations.empty())
    {
        std::cout << "You don't have incoming invitations" << std::endl;
    }
    else
    {
        std::cout << "This is the list of your incoming invitations:\n";
        for (int i{}; i < invitations.size(); ++i)
        {
            std::cout << "Invitation #" << i + 1 << ' '
                << invitations[i] << '\n';
        }
    }
}

void doPollInvitations(httplib::SSLClient& client,
    std::deque<Invitation>& invitations, std::mutex& invitationsMutex,
    std::atomic<bool>& gotUserInput, std::atomic<bool>& currentlyResponding)
{
    while (!gotUserInput)
    {
        try
        {
            std::deque<Invitation> invitationsLatest{pollInvitations(client)};
            if (invitations != invitationsLatest)
            {
                while (currentlyResponding)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(25));
                }
                std::lock_guard<std::mutex> lock(invitationsMutex);
                invitations = invitationsLatest;
                if (gotUserInput) return;
                std::cout << "The list of invitations was updated"
                    << std::endl;
                printInvitations(invitations);
                std::cout << "Enter the number of invitation or e to exit"
                    << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "Could not update the list of incoming invitations"
                << std::endl;
        }
    }
}

Invitation askForInvitationToRespondTo(std::deque<Invitation>& inviatations,
    std::atomic<bool>& currentlyResponding, httplib::SSLClient& client)
{
    while (true)
    {
        std::string userInput{getString(
            "Enter the number of invitation or e to exit")};

        if (userInput == "e") return {};

        int invitationNumber{};
        std::istringstream userInputStream{userInput};
        if (!(userInputStream >> invitationNumber))
        {
            printErrorMessage();
            continue;
        }

        if ((invitationNumber > inviatations.size())||
            (invitationNumber <= 0))
        {
            printErrorMessage();
        }
        else
        {
            currentlyResponding = true;
            std::cout << "You are respoding to invitation " <<
                inviatations[invitationNumber - 1] << std::endl;
            while (true)
            {
                char userResponse{getChar(
                    "Enter a to accept or d to decline the invitation")};
                if (userResponse == 'a')
                {
                    currentlyResponding = false;
                    return {inviatations[invitationNumber - 1]};
                }
                else if (userResponse == 'd')
                {
                    try
                    {
                        declineInvitation(client,
                            inviatations[invitationNumber - 1].id);
                        std::cout << "Invitation declined" << std::endl;
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                    currentlyResponding = false;
                    break;
                }
                else
                {
                    printErrorMessage();
                }
            }
        }
    }
}

void wait(httplib::SSLClient& client)
{
    startWaiting(client);
    
    std::deque<Invitation> invitations{pollInvitations(client)};
    printInvitations(invitations);

    std::atomic<bool> gotUserInput{false};
    std::atomic<bool> currentlyResponding{false};
    std::mutex invitationsMutex;

    std::thread pollServer(doPollInvitations, std::ref(client),
                           std::ref(invitations), std::ref(invitationsMutex),
                           std::ref(gotUserInput),
                           std::ref(currentlyResponding));

    Invitation choosenInvitation{askForInvitationToRespondTo(
        invitations, currentlyResponding, client)};
    gotUserInput = true;
    pollServer.detach();

    if (choosenInvitation == Invitation{}) return;
    
    try
    {
        std::string gameId{acceptInvitation(client, choosenInvitation.id)};
        startOnline(client, gameId, choosenInvitation.gridSize,
            choosenInvitation.winningLine,
            (choosenInvitation.inviterPlayingX ? States::o : States::x));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void matchmakingMenu(httplib::SSLClient& client)
{
    while (true)
    {
        try
        {
            char userAction{getChar(
                "Enter i to invite other user, w to wait for invitation, "
                "or e to exit to the main menu")};
            switch (userAction)
            {
                case 'i': invite(client); break;
                case 'w': wait(client); break;
                case 'e': return;
                default: printErrorMessage(); break;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void loginMenu(httplib::SSLClient& client)
{
    while (true)
    {
        try
        {
            char userAction{getChar(
                "Enter l to log in, c to create account, d to delete account "
                "or e to exit to the main menu")};
            switch (userAction)
            {
                case 'l': login(client, askForUserCredentials());
                          std::cout << "Login successful" << std::endl;
                          matchmakingMenu(client); return;
                case 'c': createAccount(client, askForUserCredentials());
                          std::cout << "Account created" << std::endl;
                          matchmakingMenu(client); return;
                case 'd': deleteAccount(client, askForUserCredentials());
                          std::cout << "Account deleted" << std::endl; break;
                case 'e': return;
                default: printErrorMessage(); break;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

bool validateAdress(httplib::SSLClient& client)
{
    try
    {
        return checkAvailability(client);
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

void online()
{
    httplib::SSLClient client{getServerAdress()};
    client.set_write_timeout(std::chrono::seconds(30));
    if (validateAdress(client))
    {
        std::cout << "Adress validated" << std::endl;
    }
    else
    {
        std::cerr << "Could not validate server adress" << std::endl;
        return;
    }

    loginMenu(client);
}