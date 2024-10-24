from themis_backend.domain.use_cases import GetLastMessage
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class GetLastMessageController(Controller):
    def __init__(self, use_case: GetLastMessage) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        token_dto = http_request.authorization
        message = await self.__use_case.execute(token_dto)

        return HttpResponse(
            status_code=200,
            body={'data': {'message': message}},
            authorization=http_request.authorization,
        )
