from __future__ import annotations

from datetime import datetime
from typing import NamedTuple
from uuid import UUID


class CreateUserDTO(NamedTuple):
    name: str
    email: str
    password: str


class UserDTO(NamedTuple):
    id: UUID | str
    name: str
    email: str
    hashed_password: str
    created_at: datetime
    updated_at: datetime


class SignInDTO(NamedTuple):
    email: str
    password: str
