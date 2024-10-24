from typing import Optional

from themis_backend.domain.entities import User
from themis_backend.domain.repositories import UserRepository
from themis_backend.domain.services import HashService
from themis_backend.presentation.dtos import CreateUserDTO
from themis_backend.presentation.validators import CreateUserValidator


class CreateUser:
    def __init__(
        self, repository: UserRepository, hash_service: HashService
    ) -> None:
        self.__repository = repository
        self.__hash = hash_service

    async def execute(self, user_dto: CreateUserDTO) -> Optional[User]:
        validator = CreateUserValidator(user_dto)
        validator.validate()

        user = await self.__repository.create(
            name=user_dto.name.title(),
            email=user_dto.email,
            hashed_password=self.__hash.hash(user_dto.password),
        )

        return user
