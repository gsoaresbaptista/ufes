from themis_backend.domain.use_cases import AuthenticateUser
from themis_backend.presentation.http import HttpRequest, HTTPUnauthorized

from .controller import Controller


class AuthenticateUserController(Controller):
    def __init__(self, use_case: AuthenticateUser) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpRequest:
        token = http_request.header.get('Authorization', None)
        token = await self.__use_case.execute(token)

        if token is None:
            raise HTTPUnauthorized()

        http_request.authorization = token

        return http_request
