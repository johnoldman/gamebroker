#include "SglExceptions.h"

typedef void (*BeginUserSessionCallback)(const std::string &gameSessionId,
                                         long userId,
                                         const std::string &userSessionId,
                                         long gameCost,
                                         long gameFloorLimit,
                                         long purseTotal);
typedef void (*BeginUserSessionExceptionCallback)(const std::string &gameSessionId,
                                                  long userId,
                                                  const std::string &userSessionId,
                                                  long gameCost,
                                                  long gameFloorLimit,
                                         	  enum SglGameAPI::SglServerExceptionType SglServerException,
                                         	  const std::string& SglServerExceptionDescription);

typedef void (*EndUserSessionCallback)(const std::string &messageId,
                                       const std::string &gameSessionId,
                                       const std::string &userSessionId);
typedef void (*EndUserSessionErrorCallback)(const std::string &messageId,
                                            const std::string &gameSessionId,
                                            const std::string &userSessionId,
                                            enum SglGameAPI::SglServerExceptionType SglServerException,
                                            const std::string &SglServerExceptionDescription);

typedef void (*MakePaymentCallback)(const std::string &messageId,
                                    const std::string &gameSessionId,
                                    const std::string &fromUserSessionId,
                                    const std::string &toUserSessionId,
                                    long amount,
                                    const std::string &eventCode);
typedef void (*MakePaymentErrorCallback)(const std::string &messageId,
                                         const std::string &gameSessionId,
                                         const std::string &fromUserSessionId,
                                         const std::string &toUserSessionId,
                                         long amount,
                                         const std::string &eventCode,
                                         enum SglGameAPI::SglServerExceptionType SglServerException,
                                         const std::string &SglServerExceptionDescription);

typedef void (*MakeSystemPaymentCallback)(const std::string &messageId,
                                          const std::string &gameSessionId,
                                          const std::string &userSessionId,
                                          bool payUser,
                                          long amount,
                                          const std::string &totalType,
                                          const std::string &totalClassification,
                                          const std::string &eventCode);
typedef void (*MakeSystemPaymentErrorCallback)(const std::string &messageId,
                                               const std::string &gameSessionId,
                                               const std::string &userSessionId,
                                               bool payUser,
                                               long amount,
                                               const std::string &totalType,
                                               const std::string &totalClassification,
                                               const std::string &eventCode,
                                               enum SglGameAPI::SglServerExceptionType SglServerException,
                                               const std::string &SglServerExceptionDescription);

typedef void (*GetUserDataCallback)(const std::string &messageId,
                                    const std::string &gameSessionId,
                                    long userId,
                                    const std::string &userDataType,
                                    const std::string &userData);
typedef void (*GetUserDataErrorCallback)(const std::string &messageId,
                                        const std::string &gameSessionId,
                                        long userId,
                                        const std::string &userDataType,
                                        enum SglGameAPI::SglServerExceptionType SglServerException,
                                        const std::string &SglServerExceptionDescription);

typedef void (*SetUserDataCallback)(const std::string &messageId,
                                    const std::string &gameSessionId,
                                    long userId,
                                    const std::string &userDataType,
                                    const std::string &userData);
typedef void (*SetUserDataErrorCallback)(const std::string &messageId,
                                         const std::string &gameSessionId,
                                         long userId,
                                         const std::string &userDataType,
                                         const std::string &userData,
                                         enum SglGameAPI::SglServerExceptionType SglServerException,
                                         const std::string &SglServerExceptionDescription);
