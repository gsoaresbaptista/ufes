from themis_backend.domain.use_cases import SignIn
from themis_backend.external.repositories import (
    PostgreRefreshTokenRepository,
    PostgreUserRepository,
)
from themis_backend.external.services import (
    BcryptHashService,
    JTWAccessTokenService,
)
from themis_backend.presentation.controllers import (
    Controller,
    SignInController,
)


def sign_in_composer() -> Controller:
    user_repository = PostgreUserRepository()
    refresh_repository = PostgreRefreshTokenRepository()
    hash_service = BcryptHashService()
    token_service = JTWAccessTokenService()
    use_case = SignIn(
        user_repository, hash_service, token_service, refresh_repository
    )
    controller = SignInController(use_case)
    return controller
