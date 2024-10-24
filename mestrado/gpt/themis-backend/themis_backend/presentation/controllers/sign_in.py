from themis_backend.domain.use_cases import SignIn
from themis_backend.presentation.dtos import SignInDTO
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class SignInController(Controller):
    def __init__(self, use_case: SignIn) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        email = http_request.body.get('email', None)
        password = http_request.body.get('password', None)

        dto = SignInDTO(email=email, password=password)
        authorization_header = await self.__use_case.execute(dto)

        return HttpResponse(
            status_code=200,
            body={
                'data': {
                    'access_token': authorization_header.access_token,
                    'refresh_token': authorization_header.refresh_token,
                    'user': authorization_header.user,
                }
            },
        )
