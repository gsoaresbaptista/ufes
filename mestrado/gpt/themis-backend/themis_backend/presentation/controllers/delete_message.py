from themis_backend.domain.use_cases import DeleteMessage
from themis_backend.presentation.dtos import DeleteMessageDTO
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class DeleteMessageController(Controller):
    def __init__(self, use_case: DeleteMessage) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        delete_dto = DeleteMessageDTO(
            user_id=http_request.authorization.user_id,
            message_id=http_request.body.get('message_id'),
        )
        message = await self.__use_case.execute(delete_dto)

        return HttpResponse(
            status_code=200,
            body={'data': {'message': message}},
        )
