from themis_backend.domain.use_cases import GetMessages
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class GetMessagesController(Controller):
    def __init__(self, use_case: GetMessages) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        token_dto = http_request.authorization
        messages = await self.__use_case.execute(token_dto)

        return HttpResponse(
            status_code=200,
            body={'data': {'messages': messages}},
            authorization=http_request.authorization,
        )
