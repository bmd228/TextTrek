#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"
#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp-swagger/Controller.hpp"
#include <oatpp-1.3.0/oatpp/oatpp/network/Address.hpp>
#include "../workers/main_worker.h"
/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
    //AppComponent(std::shared_ptr<MainWorker> worker_)
    //{
    //    OATPP_CREATE_COMPONENT(std::shared_ptr<MainWorker>, worker)([&worker_] {
    //        return worker_; // Возвращаем указатель на объект worker_
    //        }());
    //}
  /**
   * Create Async Executor
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
    return std::make_shared<oatpp::async::Executor>(
      4 /* Data-Processing threads */,
      1 /* I/O threads */,
      1 /* Timer threads */
    );
  }());
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  //OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
  //  return oatpp::network::tcp::server::ConnectionProvider::createShared({ "localhost", 8000, oatpp::network::Address::IP_4 });
  //}());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  //OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
  //  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
  //  OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Async executor component
  //  return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
  //}());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::swagger::DocumentInfo>,
      swaggerDocumentInfo
  )([] {

      oatpp::swagger::DocumentInfo::Builder builder;
      builder
          .setTitle("My Demo Service with Swagger-UI")
          .setDescription("C++/oat++ Web Service with Swagger-UI")
          .setVersion("1.0")
          .setContactName("Mr. Developer")
          .setContactUrl("https://oatpp.io/")
          .setLicenseName("Apache License, Version 2.0")
          .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
          .addServer("http://localhost:8000", "server on localhost");
      return builder.build();
      }());

  /**
  *  Swagger-Ui Resources
  */


  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::swagger::Resources>,
      swaggerResources
  )([] {
      return oatpp::swagger::Resources::loadResources(
          OATPP_SWAGGER_RES_PATH
      );
      }());
  
  
};

#endif /* AppComponent_hpp */
