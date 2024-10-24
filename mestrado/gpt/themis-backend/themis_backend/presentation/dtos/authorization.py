from typing import NamedTuple
from uuid import UUID

from .user import UserDTO


class TokenDTO(NamedTuple):
    user_id: UUID | str
    name: str
    email: str


class AuthorizationHeaderDTO(NamedTuple):
    access_token: str
    refresh_token: dict[str, str]
    user: UserDTO | None


class RefreshTokenDTO(NamedTuple):
    refresh_token: str
