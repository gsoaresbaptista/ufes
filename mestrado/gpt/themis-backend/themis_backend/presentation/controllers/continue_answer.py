from themis_backend.domain.use_cases import ContinueAnswer
from themis_backend.presentation.dtos import ContinueAnswerDTO
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class ContinueAnswerController(Controller):
    def __init__(self, use_case: ContinueAnswer) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        continue_dto = ContinueAnswerDTO(
            message_id=http_request.body.get('message-id', None),
            user_id=http_request.authorization.user_id,
        )

        message = await self.__use_case.execute(continue_dto)

        return message, http_request.authorization
