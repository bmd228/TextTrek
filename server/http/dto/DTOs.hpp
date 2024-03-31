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

	DTO_FIELD(Enum<State>::AsString, state)=State::OK;
	DTO_FIELD(String, err);
	DTO_FIELD(String, pData);
	DTO_FIELD(String, language);
	public:

		ResponseDto() = default;

		ResponseDto(const String& pData_, const String& language_, const String& err_,const Enum<State>& state_ )
			: pData(pData_)
			, language(language_)
			, err(err_),
			state(state_)
		{}
		ResponseDto(const MessageResponse& message)
			: pData(message.pData)
			, language(message.language)
			, err(message.error),
			state(State(message.state))
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

};
#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
