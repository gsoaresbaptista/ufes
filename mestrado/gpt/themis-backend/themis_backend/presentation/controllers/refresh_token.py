from themis_backend.domain.use_cases import RefreshToken
from themis_backend.presentation.dtos import RefreshTokenDTO
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class RefreshTokenController(Controller):
    def __init__(self, use_case: RefreshToken) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        refresh_token = http_request.body.get('refresh-token', None)
        refresh_token = RefreshTokenDTO(refresh_token=refresh_token)
        authorization_header = await self.__use_case.execute(refresh_token)

        return HttpResponse(
            status_code=200,
            body={
                'data': {
                    'access_token': authorization_header.access_token,
                    'refresh_token': authorization_header.refresh_token,
                }
            },
        )
