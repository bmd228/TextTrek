#ifndef MyController_hpp
#define MyController_hpp

#include "../dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

#include "oatpp/web/protocol/http/outgoing/MultipartBody.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"
#include "oatpp/web/mime/multipart/InMemoryDataProvider.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"
//#include "oatpp/core/utils/ConversionUtils.hpp"
#include "../../workers/main_worker.h"


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen
namespace multipart = oatpp::web::mime::multipart;
/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
private:
  typedef MyController __ControllerType;
  
public:
    MainWorker& m_mainWorker;
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  MyController(MainWorker& mainWorker,OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper), m_mainWorker(mainWorker)
  {
      
  }
public:
    static std::shared_ptr<MyController> createShared(
        MainWorker& mainWorker, OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
    ) {
        return std::make_shared<MyController>(mainWorker,objectMapper);
    }
    
    ENDPOINT_INFO(Worker) {
        info->summary = "Create new User";
        
        info->addConsumes<Object<RequestDto>>("multipart/form-data");
        info->addResponse<Object<ResponseDto>>(Status::CODE_200,"application/json");
        //info->pathParams.add<String>("param", "Parameter description");
    }
  ENDPOINT_ASYNC("POST", "worker", Worker, REQUEST(std::shared_ptr<IncomingRequest>, request)) {


    ENDPOINT_ASYNC_INIT(Worker)

    std::shared_ptr<multipart::PartList> m_multipart;

    Action act() override {      
        
        /* Prepare multipart container. */
        m_multipart = std::make_shared<multipart::PartList>(request->getHeaders());
        //m_multipart = std::make_shared<multipart::Multipart>(request->getHeaders());
        auto multipartReader = std::make_shared<multipart::AsyncReader>(m_multipart);
        multipartReader->setPartReader("file", multipart::createAsyncInMemoryPartReader(-1 /* max-data-size */));
        multipartReader->setPartReader("format", multipart::createAsyncInMemoryPartReader(1024 /* max-data-size */));
        multipartReader->setPartReader("language", multipart::createAsyncInMemoryPartReader(1024 /* max-data-size */));
        return request->transferBodyAsync(multipartReader).next(yieldTo(&Worker::onUploaded));


    }
    Action onUploaded() {

        /* Get multipart by name */
        auto file = m_multipart->getNamedPart("file");
        auto format = m_multipart->getNamedPart("format");
        auto language = m_multipart->getNamedPart("language");

        OATPP_ASSERT_HTTP(format, Status::CODE_400, "file is null");
        OATPP_ASSERT_HTTP(file, Status::CODE_400, "file is null");
        auto message = MessageRequest(file->getPayload()->getInMemoryData(), language->getPayload()->getInMemoryData(), MessageRequest::stringToFormat(format->getPayload()->getInMemoryData()));
        auto response = ResponseDto::createShared(controller->m_mainWorker.push(message));
        return _return(controller->createDtoResponse(Status::CODE_200, response));

    }
  };

  // TODO Insert Your endpoints here !!!
  
};

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- End codegen

#endif /* MyController_hpp */
