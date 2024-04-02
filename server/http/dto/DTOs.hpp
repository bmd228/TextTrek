#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp-swagger/Types.hpp"
#include "../../workers/main_worker.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
ENUM(State, v_uint8,
	VALUE(OK, 0),
	VALUE(Error, 1)
)
class ResponseDto : public oatpp::DTO {

	DTO_INIT(ResponseDto, DTO);

	DTO_FIELD(String, pData);
	DTO_FIELD(String, pData_recognize);
	DTO_FIELD(String, language);
	public:

		ResponseDto() = default;

		ResponseDto(const String& pData_, const String& pData_recognize_, const String& language_, const String& err_,const Enum<State>& state_ )
			: pData(pData_)
			, pData_recognize(pData_recognize_)
			, language(language_)
		{}
		ResponseDto(const MessageResponse& message)
			: pData(message.pdata())
			, pData_recognize(message.pdata_recognize())
			, language(message.language())
		{
		}


};
ENUM(Formats, v_uint8,
	VALUE(AUTO, 0),
	VALUE(IMAGE, 1),
	VALUE(PDF, 2)
)

class  RequestDto : public oatpp::DTO {
	
	DTO_INIT(RequestDto, DTO);
	DTO_FIELD(Enum<Formats>, format)=Formats::AUTO;
	DTO_FIELD(oatpp::swagger::Binary, file);
	DTO_FIELD(String, language);
//public:
//	MessageRequest to_Message() {
//		MessageRequest req;
//		//
//		//static const std::map<std::string, Formats> formatMap = {
//	 //  {"AUTO", Formats::AUTO},
//	 //  {"IMAGE", Formats::IMAGE},
//	 //  {"PDF", Formats::PDF}
//	 //  // Добавьте другие соответствия, если необходимо
//		//};
//
//		//// Ищем соответствие строки в карте
//		//auto it = formatMap.find(str);
//		//if (it != formatMap.end()) {
//		//	return it->second;
//		//}
//
//		//// Если соответствие не найдено, возвращаем значение по умолчанию
//		//return Formats::AUTO;
//		req.set_format((MessageRequest::Formats)format.getEntryByName("AUTO").value);
//		req.set_pdata(file.)
//	}

};
#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
