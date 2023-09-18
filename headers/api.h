#ifndef API_H
#define API_H

#include <string>
#include <vector>
#include <deque>
#include <exception>
#include <stdexcept>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <openssl/ssl.h>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "structures.h"

bool checkAvailability(httplib::SSLClient& client);

void login(httplib::SSLClient& client,
    const std::pair<std::string, std::string>& credentials);

void createAccount(httplib::SSLClient& client,
    const std::pair<std::string, std::string>& credentials);

void deleteAccount(httplib::SSLClient& client,
    const std::pair<std::string, std::string>& credentials);

void startWaiting(httplib::SSLClient& client);

void stopWaiting(httplib::SSLClient& client);

std::vector<std::string> getWaitingUsers(httplib::SSLClient& client);

std::string sendInvitation(httplib::SSLClient& client,
    const std::string& invited,
    std::pair<int, int> gridProperties, bool playingX,
    const std::string& PlayAgainScheme);

InivtationStatus pollInvitationStatus(httplib::SSLClient& client,
    const std::string& invitationId);

void cancelInvitation(httplib::SSLClient& client,
    const std::string& invitationId);

std::deque<Invitation> pollInvitations(httplib::SSLClient& client);

std::string acceptInvitation(httplib::SSLClient& client,
    const std::string& invitationId);

void declineInvitation(httplib::SSLClient& client,
    const std::string& invitationId);

void makeMove(httplib::SSLClient& client,
    const std::string& gameId, const std::string& cell);

std::string pollGame(httplib::SSLClient& client, const std::string& gameId);

PlayAgainStatus acceptPlayAgain(
    httplib::SSLClient& client, const std::string& gameId);

void declinePlayAgain(httplib::SSLClient& client, const std::string& gameId);

PlayAgainStatus pollPlayAgainStatus(
    httplib::SSLClient& client, const std::string& gameId);

#endif