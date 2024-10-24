from themis_backend.domain.use_cases import RefreshToken
from themis_backend.external.repositories import (
    PostgreRefreshTokenRepository,
    PostgreUserRepository,
)
from themis_backend.external.services import JTWAccessTokenService
from themis_backend.presentation.controllers import (
    Controller,
    RefreshTokenController,
)


def refresh_token_composer() -> Controller:
    user_repository = PostgreUserRepository()
    refresh_token_repository = PostgreRefreshTokenRepository()
    token_service = JTWAccessTokenService()

    use_case = RefreshToken(
        refresh_token_repository, user_repository, token_service
    )
    controller = RefreshTokenController(use_case)
    return controller
