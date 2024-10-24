from datetime import datetime

from themis_backend.domain.entities import RefreshToken
from themis_backend.domain.repositories import (
    RefreshTokenRepository,
    UserRepository,
)
from themis_backend.domain.services import AccessTokenService, HashService
from themis_backend.presentation.dtos import AuthorizationHeaderDTO, SignInDTO
from themis_backend.presentation.http.errors import (
    IncorrectPassword,
    UserNotFound,
)
from themis_backend.presentation.validators import SignInValidator


class SignIn:
    def __init__(
        self,
        repository: UserRepository,
        hash_service: HashService,
        token_service: AccessTokenService,
        refresh_token_repository: RefreshTokenRepository,
    ) -> None:
        self.__user_repository = repository
        self.__hash = hash_service
        self.__token = token_service
        self.__refresh_token_repository = refresh_token_repository

    async def execute(self, sign_in_dto: SignInDTO) -> AuthorizationHeaderDTO:
        validator = SignInValidator(sign_in_dto)
        validator.validate()

        user = await self.__get_user(sign_in_dto.email)
        await self.__validate_user(user, sign_in_dto.password)

        access_token = await self.__token.create(user)
        refresh_token = await self.__get_or_create_refresh_token(user)

        return AuthorizationHeaderDTO(
            access_token=access_token,
            refresh_token=refresh_token.to_dict(),
            user=user.to_dict(),
        )

    async def __get_user(self, email: str):
        user = await self.__user_repository.search_by_email(email=email)
        if not user:
            raise UserNotFound()
        return user

    async def __validate_user(self, user, password):
        if not self.__hash.compare(password, user.hashed_password):
            raise IncorrectPassword()

    async def __get_or_create_refresh_token(self, user):
        refresh_token = (
            await self.__refresh_token_repository.search_by_user_id(user.id)
        )
        to_create = refresh_token is None

        if (
            refresh_token is not None
            and refresh_token.expires_in
            < datetime.now(refresh_token.expires_in.tzinfo)
        ):
            await self.__refresh_token_repository.delete_all(user_id=user.id)
            to_create = True

        if to_create:
            refresh_token = await self.__refresh_token_repository.create(
                user_id=user.id
            )

        return RefreshToken(
            id=refresh_token.id,
            user_id=refresh_token.id,
            expires_in=refresh_token.expires_in,
        )
