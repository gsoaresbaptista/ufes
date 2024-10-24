from themis_backend.domain.use_cases import ClearChat
from themis_backend.presentation.dtos import ClearChatDTO
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class ClearChatController(Controller):
    def __init__(self, use_case: ClearChat) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        clear_dto = ClearChatDTO(user_id=http_request.authorization.user_id)
        messages = await self.__use_case.execute(clear_dto)

        return HttpResponse(
            status_code=200,
            body={
                'data': {'messages': messages if messages is not None else []}
            },
        )
