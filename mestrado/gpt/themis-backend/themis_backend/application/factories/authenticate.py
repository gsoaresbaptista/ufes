from themis_backend.domain.use_cases import AuthenticateUser
from themis_backend.external.services import JTWAccessTokenService
from themis_backend.presentation.controllers import (
    AuthenticateUserController,
    Controller,
)


def authenticate_composer() -> Controller:
    token = JTWAccessTokenService()
    use_case = AuthenticateUser(token)
    controller = AuthenticateUserController(use_case)
    return controller
