//
// $Id: SglExceptions.h 2329 2007-11-01 15:48:49Z stephend $
//
#ifndef SGL_EXCEPTIONS_H_
#define SGL_EXCEPTIONS_H_
	
namespace SglGameAPI {
	class SglBaseException {
	public : 
		// A message saying WHY this exception has occured.
		std::string getMessage() const;
			
	protected : 
		SglBaseException() {}
		virtual ~SglBaseException() {}
	
	private :
		// Prevent the base class being copied stops the entire hierarchy.
		SglBaseException(const SglBaseException &exception);
		SglBaseException& operator=(const SglBaseException &exception);
	};

	class SglCommunicationError : public SglBaseException {
		protected : 
			SglCommunicationError() {}
			virtual ~SglCommunicationError() {}	
	};

	class SglGameSessionTypeNameMatchError : public SglBaseException {
		protected : 
			SglGameSessionTypeNameMatchError() {}
			virtual ~SglGameSessionTypeNameMatchError() {}
	};

	class SglUnknownGameSessionId : public SglBaseException {
		protected : 
			SglUnknownGameSessionId() {}
			virtual ~SglUnknownGameSessionId() {}	
	};	


	enum SglServerExceptionType
	{
		SglSuccess,
		SglServerUnknownGameSessionId,
		SglServerGameClosed,
		SglServerUnknownUserId,
		SglServerUnknownUserSessionId,
		SglServerInsufficientFunds,
		SglServerDuplicateUserSessionId,
		SglServerDuplicateGameSessionId,
		SglServerFundsRemaining,
		SglServerUnknownEventCode,
		SglServerUnknownDataType,
                SglServerCorruptMessage,
		SglNotInitialised,
		SglServerUnknownError
	};
}

#endif
