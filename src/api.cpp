#include "api.h"

using JSON = nlohmann::json;

void checkResponse(const httplib::Result& response)
{
    if (!response)
    {
        throw std::runtime_error("Connection error");
    }

    if (response->status != 200)
    {
        if (response->body.empty())
        {
            throw std::runtime_error("Unknown error");
        }
        else
        {
            JSON parsedResponse = JSON::parse(response->body);
            if (parsedResponse.contains("detail"))
            {
                throw std::runtime_error(parsedResponse["detail"]);
            }
            else
            {
                throw std::runtime_error("Unknown error");
            }
        }
    }
}

bool checkAvailability(httplib::SSLClient& client)
{
    auto response{client.Get("/check_availability")};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    return parsedResponse["message"] == "Tic-tac-toe server is available here";
}

void login(httplib::SSLClient& client,
    const std::pair<std::string, std::string>& credentials)
{
    httplib::Headers basicAuthHeader{
        {"Authorization", "Basic " + httplib::detail::base64_encode(
            credentials.first + ":" + credentials.second)}
    };

    auto response{client.Post("/login", basicAuthHeader)};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    client.set_bearer_token_auth(parsedResponse["token"]);
}

void createAccount(httplib::SSLClient& client,
    const std::pair<std::string, std::string>& credentials)
{
    httplib::Headers basicAuthHeader{
        {"Authorization", "Basic " + httplib::detail::base64_encode(
            credentials.first + ":" + credentials.second)}
    };

    auto response{client.Post("/create_user", basicAuthHeader)};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    client.set_bearer_token_auth(parsedResponse["token"]);
}

void deleteAccount(httplib::SSLClient& client,
    const std::pair<std::string, std::string>& credentials)
{
    httplib::Headers basicAuthHeader{
        {"Authorization", "Basic " + httplib::detail::base64_encode(
            credentials.first + ":" + credentials.second)}
    };
    
    auto response{client.Delete("/delete_account", basicAuthHeader)};
    checkResponse(response);
}

void startWaiting(httplib::SSLClient& client)
{
    auto response{client.Post("/start_waiting")};
    checkResponse(response);
}

void stopWaiting(httplib::SSLClient& client)
{
    auto response{client.Post("/stop_waiting")};
    checkResponse(response);
}

std::vector<std::string> getWaitingUsers(httplib::SSLClient& client)
{
    auto response{client.Get("/waiting_users")};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    return parsedResponse["waiting_users"];
}

std::string sendInvitation(httplib::SSLClient& client,
    const std::string& invited,
    std::pair<int, int> gridProperties, bool playingX,
    const std::string& PlayAgainScheme)
{
    JSON requestBody{
        {"invited", invited},
        {"grid_properties", {
            {"size", gridProperties.first},
            {"winning_line", gridProperties.second}
        }},
        {"inviter_playing_x", playingX},
        {"play_again_scheme", PlayAgainScheme}
    };

    auto response{client.Post(
        "/invite", requestBody.dump(), "application/json")};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    return parsedResponse["invitation_id"];
}

InivtationStatus pollInvitationStatus(httplib::SSLClient& client,
    const std::string& invitationId)
{
    auto response{client.Get(
        "/poll_invitation_status?invitation_id=" + invitationId)};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    std::string status{parsedResponse["status"]};
    std::string gameId{""};
    if (status == "accepted") gameId = parsedResponse["game_id"];
    return {status, gameId};
}

void cancelInvitation(httplib::SSLClient& client,
    const std::string& invitationId)
{
    auto response{client.Post(
        "/cancel_invitation?invitation_id=" + invitationId)};
    checkResponse(response);
}

std::deque<Invitation> pollInvitations(httplib::SSLClient& client)
{
    auto response{client.Get("/poll_invitations")};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    std::deque<Invitation> invitations{};
    for (const auto& invitation: parsedResponse["invitations"])
    {
        std::string invitationId = invitation["invitation_id"];
        std::string inviter = invitation["inviter"];
        int gridSize = invitation["grid_properties"]["size"];
        int winningLine = invitation["grid_properties"]["winning_line"];
        bool inviterPlayingX = invitation["inviter_playing_x"];
        std::string playAgainScheme = invitation["play_again_scheme"];

        invitations.push_back({invitationId, inviter, gridSize, winningLine,
            inviterPlayingX, playAgainScheme});
    }
    return invitations;
}

std::string acceptInvitation(httplib::SSLClient& client,
    const std::string& invitationId)
{
    JSON requestBody{
        {"invitation_id", invitationId},
        {"response", "accept"}
    };

    auto response{client.Post(
        "/respond_invitation", requestBody.dump(), "application/json")};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    return parsedResponse["game_id"];
}

void declineInvitation(httplib::SSLClient& client,
    const std::string& invitationId)
{
    JSON requestBody{
        {"invitation_id", invitationId},
        {"response", "decline"}
    };

    auto response{client.Post(
        "/respond_invitation", requestBody.dump(), "application/json")};
    checkResponse(response);
}

void makeMove(httplib::SSLClient& client,
    const std::string& gameId, const std::string& cell)
{
    JSON requestBody{
        {"game_id", gameId},
        {"cell", cell}
    };

    auto response{client.Post(
        "/make_move", requestBody.dump(), "application/json")};
    checkResponse(response);
}

std::string pollGame(httplib::SSLClient& client, const std::string& gameId)
{
    auto response{client.Get("/poll_game?game_id=" + gameId)};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    if (parsedResponse["new_move"])
    {
        return parsedResponse["cell"];
    }
    else
    {
        return "";
    }
}

PlayAgainStatus acceptPlayAgain(
    httplib::SSLClient& client, const std::string& gameId)
{
    JSON requestBody{
        {"game_id", gameId},
        {"play_again", true}
    };

    auto response{client.Post(
        "/play_again", requestBody.dump(), "application/json")};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    if (parsedResponse["status"] == "Waiting for opponent to accept")
    {
        return {PlayAgainStatus::Status::waiting, "", false};
    }
    else if (parsedResponse["status"] == "New game started")
    {
        return {PlayAgainStatus::Status::accepted,
            parsedResponse["new_game_id"], parsedResponse["switch_sides"]};
    }
    else
    {
        throw std::runtime_error("Unknown status");
    }
}

void declinePlayAgain(httplib::SSLClient& client, const std::string& gameId)
{
    JSON requestBody{
        {"game_id", gameId},
        {"play_again", false}
    };

    auto response{client.Post(
        "/play_again", requestBody.dump(), "application/json")};
    checkResponse(response);
}

PlayAgainStatus pollPlayAgainStatus(
    httplib::SSLClient& client, const std::string& gameId)
{
    auto response{client.Get(
        "/poll_play_again_status?game_id=" + gameId)};
    checkResponse(response);
    JSON parsedResponse = JSON::parse(response->body);
    if (parsedResponse["play_again_status"] == "accepted")
    {
        return {PlayAgainStatus::Status::accepted,
            parsedResponse["next_game_id"], parsedResponse["switch_sides"]};
    }
    else if (parsedResponse["play_again_status"] == "declined")
    {
        return {PlayAgainStatus::Status::declined, "", false};
    }
    else if ((parsedResponse["play_again_status"] == "requested_by_x") ||
        (parsedResponse["play_again_status"] == "requested_by_o"))
    {
        return {PlayAgainStatus::Status::waiting, "", false}; 
    }
    else
    {
        throw std::runtime_error("Unknown status");
    }
}