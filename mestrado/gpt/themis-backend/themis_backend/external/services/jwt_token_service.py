from datetime import datetime, timedelta
from typing import Optional

from jose import JWTError, jwt

from themis_backend.config import TokenSettings
from themis_backend.domain.entities import User
from themis_backend.domain.services import AccessTokenService
from themis_backend.presentation.dtos import TokenDTO


class JTWAccessTokenService(AccessTokenService):
    def __init__(self) -> None:
        self.__expire_minutes = TokenSettings.EXPIRE_MINUTES
        self.__algorithm = TokenSettings.ALGORITHM
        self.__secret_key = TokenSettings.SECRET_KEY

    async def create(self, user: User) -> str:

        data = user.to_dict()
        expire = datetime.utcnow() + timedelta(minutes=self.__expire_minutes)
        data.update({'exp': expire})
        encoded_jwt = jwt.encode(
            data, self.__secret_key, algorithm=self.__algorithm
        )

        return encoded_jwt

    async def decode(self, token: str) -> Optional[TokenDTO]:
        try:
            payload = jwt.decode(
                token, key=self.__secret_key, algorithms=self.__algorithm
            )
            return TokenDTO(
                user_id=payload['id'],
                name=payload['name'],
                email=payload['email'],
            )
        except JWTError:
            return None
