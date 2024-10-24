from themis_backend.domain.use_cases import CreateUser
from themis_backend.external.repositories import PostgreUserRepository
from themis_backend.external.services import BcryptHashService
from themis_backend.presentation.controllers import (
    Controller,
    CreateUserController,
)


def create_user_composer() -> Controller:
    repository = PostgreUserRepository()
    hash_service = BcryptHashService()
    use_case = CreateUser(repository, hash_service)
    controller = CreateUserController(use_case)
    return controller
