from typing import Optional

from themis_backend.domain.services import AccessTokenService
from themis_backend.presentation.dtos import UserDTO


class AuthenticateUser:
    def __init__(self, token_service: AccessTokenService) -> None:
        self.__token = token_service

    async def execute(self, bearer_token: str) -> Optional[UserDTO]:
        if bearer_token is None:
            return None
        token = bearer_token[7:]  # ignore text 'Bearer '
        return await self.__token.decode(token)
