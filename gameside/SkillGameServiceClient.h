//
// $Id: SkillGameServiceClient.h 3377 2007-12-06 17:05:55Z johno $
//
#ifndef SGL_GAME_API_H_
#define SGL_GAME_API_H_

#include "ApiCallbackDefs.h"
#include "SglExceptions.h"
#include "Config.h"

namespace SglGameAPI {
	// Don't put throw specifications in C++ - unfortunately they don't do the same as they do
	// in Java :-(
	class GameClient
	{
	public :
                static void initialise(const Config& config);
		/**
		 * This method registers a game instance, it will also flag if the game instance is running,
		 *  this method will accept the same parameters.
		 *
		 * @param gameName - The name of the game (e.g. "METTLE")
		 * @param gameType - The type of game to be played (e.g. "Tournament");
		 * @param gameSessionId - A unique game session id, used by the game server calls to identify which
		 *                         game session is currently in use.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 * @throws SglGameSessionTypeNameMatchError - A gameSessionId was used with a different game name and type.
		 */
		static void registerGameInstance(const std::string &gameName,
		                                 const std::string &gameType,
		                                 const std::string &gameSessionId,
                                       const std::string &gameTariffId);
//			throw (SglCommunicationError, SglGameSessionTypeNameMatchError);




		/**
		 * This method deregisters a game instance, it will also flag if the game instance is running,
		 *  this method will accept the same parameters.
		 *
		 * @param gameName - The name of the game (e.g. "METTLE")
		 * @param gameType - The type of game to be played (e.g. "Tournament");
		 * @param gameSessionId - A unique game session id, used by the game server calls to identify which
		 *                         game session is currently in use.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 * @throws SglUnknownGameSessionId - A gameSessionId was used with a different game name and type.
		 * @throws SglGameClosed - The game instance specified is already closed.
		 */
		static void deregisterGameInstance(const std::string &gameSessionId);
//				throw (SglCommunicationError, SglUnknownGameSessionId, SglGameClosed);

		/**
		 * This method sets up a user session (start a game, and pay for that game).
		 *
		 * @param messageId - A unique message id, if this exact message is replayed (i.e. for replay), it MUST
		 *                     have the same message id, else the message action (this function) will be re-performed.
		 * @param gameSessionId - The game session id, that was previously used to open this game.
		 * @param userId - The SGLUser (persona), for this mettle player.
		 * @param userSessionId - A unique id for this user in this game instance, for this game play.
		 * @param gameCost - The cost of this game.
		 * @param gameFloorLimit - The total amount (including game cost), that must be withdrawn from the users Wallet
		 *                          and put into the purse.
		 *
		 * @return The total left in the purse (this will NO LONGER contain the gameCost).
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned from server
		 * @throws SglServerUnknownGameSessionId - A gameSessionId was used with a different game name and type.
		 * @throws SglServerGameClosed - The game instance specified is now closed.
		 * @throws SglServerUnknownUserId - The SGLUser Id used is not known, or is not valid for the game used.
		 * @throws SglServerInsufficiantFunds - The user does not have the total funds required to meet the gameFloorLimit.
		 * @throws SglServerDuplicateUserSessionId - A begin user session event has already been performed using this userSessionId,
		 *                                      for this gameSession, but not for this user (i.e. this is not a repeated message).
		 */
		static void beginUserSession(const std::string &messageId,
                                  const std::string &gameSessionId,
                                  long userId,
                                  const std::string &userSessionId,
                                  const std::string &tarrifId,
                                  long gameCost,
                                  long gameFloorLimit,
                                  BeginUserSessionCallback dataCallback,
                                  BeginUserSessionExceptionCallback exceptionCallback);

		/**
		 * This method closes a user Session (end a game).
		 *
		 * @param messageId - A unique message id, if this exact message is replayed (i.e. for replay), it MUST
		 *                     have the same message id, else the message action (this function) will be re-performed.
		 * @param gameSessionId - The game session id, that was previously used to open this game.
		 * @param userSessionId - A unique id for this user in this game instance, for this game play.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned by server:
		 * @throws SglServerUnknownGameSessionId - A gameSessionId was used with a different game name and type.
		 * @throws SglServerGameClosed - The game instance specified is now closed.
		 * @throws SglServerFundsRemaining - Their are funds still left in the user purse.

		 */
		static void endUserSession(const std::string &messageId,
		                           const std::string &gameSessionId,
                                           const std::string &userSessionId,
                                           EndUserSessionCallback ackCallback,
                                           EndUserSessionErrorCallback exceptionCallback);
		/**
		 * This method makes a payment from one user to another user.
		 *
		 * @param messageId - A unique message id, if this exact message is replayed (i.e. for replay), it MUST
		 *                     have the same message id, else the message action (this function) will be re-performed.
		 * @param gameSessionId - The game session id, that was previously used to open this game.
		 * @param fromUserSessionId - A user session id for the game who will have money debited.
		 * @param toUserSessionId - A user session id for the game who will have mobey credited.
		 * @param amount - The amount to debit and credit.
		 * @param eventCode - The agreed event code for this payment type.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned from server:
		 * @throws SglServerUnknownGameSessionId - A gameSessionId was used with a different game name and type.
		 * @throws SglServerGameClosed - The game instance specified is now closed.
		 * @throws SglServerUnknownUserSessionId - The user session specified is unknown.
		 * @throws SglServerInsufficiantFunds - Their are insufficent funds to make this payment.
		 * @throws SglServerUnknownEventCode - The event code used is not recognised.
		 */
		static void makePayment(const std::string &messageId,
		                        const std::string &gameSessionId,
		                        const std::string &fromUserSessionId,
		                        const std::string &toUserSessionId,
                                        long amount,
                                        const std::string &eventCode,
                                        MakePaymentCallback ackCallback,
                                        MakePaymentErrorCallback exceptionCallback);
		/**
		 * This method makes a system payment.
		 *
		 * @param messageId - A unique message id, if this exact message is replayed (i.e. for replay), it MUST
		 *                     have the same message id, else the message action (this function) will be re-performed.
		 * @param gameSessionId - The game session id, that was previously used to open this game.
		 * @param userSessionId - A user session id for the game who will have money debited.
		 * @param payUser- A flag, which when TRUE means debit the system (and pay the user), FALSE means credit the
		 *                  system (and debit the user).
		 * @param amount - The amount to debit/credit.
		 * @param totalType - The system total type that is being credited/debited.
		 * @param eventCode - The agreed event code for this payment type.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned from server:
		 * @throws SglServerUnknownGameSessionId - A gameSessionId was used with a different game name and type.e
		 * @throws SglServerGameClosed - The game instance specified is now closed.
		 * @throws SglServerUnknownUserSessionId - The user session is unknstd::string &own.
		 * @throws SglServerInsufficiantFunds - Their are insufficent funds to make this payment.
		 * @throws SglServerUnknownDataType - The data type sepcified is unknown.
		 * @throws SglServerUnknownEventCode - The event code used is not recognised.
		 */
		static void makeSystemPayment(const std::string &messageId,
		                              const std::string &gameSessionId,
		                              const std::string &userSessionId,
		                              bool payUser,
		                              long amount,
		                              const std::string &totalType,
                                    const std::string &totalClassification,
		                              const std::string &eventCode,
                                    MakeSystemPaymentCallback ackCallback,
                                    MakeSystemPaymentErrorCallback exceptionCallback);



		/**
		 * This method allows the game to query the system total specified.
		 *
		 * @param messageId - A unique message id.
		 * @param gameSessionId - The game session id, that was previously used to open this game.
		 * @param totalType - The total type required.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned by server:
		 * @throws SglServerUnknownGameSessionId - A gameSessionId was used with a different game name and type.
		 * @throws SglServerGameClosed - The game instance specified is now closed.
		 * @throws SglServerUnknownDataType - The data type sepcified is unknown.
		 */
		static void querySystemTotal(const std::string &messageId,
		                             const std::string &gameSessionId,
		                             const std::string &totalType,
		                             void (*dataCallback)(const std::string &messageId,
		                                                  const std::string &gameSessionId,
		                                                  const std::string &totalType,
		                                                  long systemTotal),
		                             void (*exceptionCallback)(const std::string &messageId,
		                                                  const std::string &gameSessionId,
		                                                  const std::string &totalType,
                                                          enum SglGameAPI::SglServerExceptionType SglServerException,
                                                          std::string &SglServerExceptionDescription));


		/**
		 * This method allows the game to query the system data.
		 *
		 * @param messageId - A unique message id.
		 * @param systemDataType - The system data type.
		 * @param dataKey - The key value used to retrive the data.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned by the server:
		 * @throws SglServerUnknownDataType - The data type sepcified is unknown, or could not retrived.
		 */
		static void getSystemData(const std::string &messageId,
		                          const std::string &systemDataType,
		                          const std::string &dataKey,
                                  void (*dataCallback)(const std::string &messageId,
		                                               const std::string &systemDataType,
		                                               const std::string &dataKey,
		                                               const std::string &systemData),
		                          void (*exceptionCallback)(const std::string &messageId,
		                                               const std::string &systemDataType,
		                                               const std::string &dataKey,
                                                       enum SglGameAPI::SglServerExceptionType SglServerException,
                                                       std::string &SglServerExceptionDescription));




		/**
		 * This method allows the game to query the user data.
		 *
		 * @param messageId - A unique message id.
		 * @param gameSessionId - The game session id, that was previously used to open this game.
		 * @param userId - The game user id.
		 * @param userDataType - The system data type.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned by server:
		 * @throws SglServerUnknownGameSessionId - A gameSessionId was used with a different game name and type.
		 * @throws SglServerGameClosed - The game instance specified is now closed.
		 * @throws SglServerUnknownUserId - The SGLUser Id used is not known, or is not valid for the game used.
		 * @throws SglServerUnknownDataType - The data type sepcified is unknown, or could not retrived.
		 */
		 static void getUserData(const std::string &messageId,
		                         const std::string &gameSessionId,
		                         long userId,
		                         const std::string &userDataType,
		                         GetUserDataCallback dataCallback,
		                         GetUserDataErrorCallback exceptionCallback);
		/**
		 * This method allows the game to set the user data.
		 *
		 * @param messageId - A unique message id.
		 * @param gameSessionId - The game session id, that was previously used to open this game.
		 * @param userId - The game user id.
		 * @param userDataType - The user data type to set.
		 * @param userData - The user data to set.
		 *
		 * @throws SglCommunicationError - Generic communication error.
		 *
		 * Returned from server:
		 * @throws SglServerUnknownGameSessionId - A gameSessionId was used with a different game name and type.
		 * @throws SglServerGameClosed - The game instance specified is now closed.
		 * @throws SglServerUnknownUserId - The SGLUser Id used is not known, or is not valid for the game used.
		 * @throws SglServerUnknownDataType - The data type sepcified is unknown, or could not retrived.
		 */
		static void setUserData(const std::string &messageId,
		                        const std::string &gameSessionId,
		                        long userId,
		                        const std::string &userDataType,
		                        const std::string &userData,
		                        SetUserDataCallback cb,
		                        SetUserDataErrorCallback exceptionCallback);

      /**
       * This method returns a unique message id that can be used as a parameter
       * for the above API calls.
       *
       * @returns string containing a unique message id
       *
       * @throws SglUninitialisedException - The game implementation was not initialised.
       *
       */
      static std::string getUniqueMessageId();


		/**
		* Returns the number of un-acknowledged replies in the message stack.

                //void GameClient::getOutstandingReplies()
                //{
                //}
		*
		* @returns number of outstanding replies
		*
		*/
      // No longer needed.
		//static long getOutstandingRepliesLength();

	// This 'class' is more like a namespace, so there is no need to provide
	// any construction ability - instances don't make sense.
	private:
		GameClient();
		~GameClient();
		GameClient(const GameClient &client);
		//static void processMessage(const std::string &messageId, const std::string &message);
		//static void itos(const long arg, const std::string &stringVersion);
	};
};

#endif // SGL_GAME_API_H_
