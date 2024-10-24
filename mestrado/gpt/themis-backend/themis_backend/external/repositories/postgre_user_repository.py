from typing import Optional
from uuid import UUID

from sqlalchemy import select
from sqlalchemy.exc import IntegrityError

from themis_backend.domain.entities import User
from themis_backend.domain.repositories import UserRepository
from themis_backend.infra.database import Session
from themis_backend.infra.schemas import UserSchema
from themis_backend.presentation.http import UserAlreadyExists


def user_row_to_entity(row: UserSchema) -> Optional[User]:
    if row:
        return User(
            id=row.id,
            name=row.name,
            email=row.email,
            hashed_password=row.hashed_password,
            created_at=row.created_at,
            updated_at=row.updated_at,
        )
    return None


class PostgreUserRepository(UserRepository):
    async def create(
        self, name: str, email: str, hashed_password: bytes
    ) -> Optional[User]:

        user = UserSchema(
            name=name,
            email=email,
            hashed_password=hashed_password,
        )

        try:
            async with Session() as session:
                session.add(user)
                await session.commit()
                await session.refresh(user)
        except IntegrityError as exception:
            if 'violates unique constraint' in str(exception.orig):
                raise UserAlreadyExists(email=email) from exception
            raise

        return user_row_to_entity(user)

    async def search_by_email(self, email: str) -> Optional[User]:
        async with Session() as session:
            query = select(UserSchema).where(UserSchema.email == email)
            user = await session.execute(query)

        return user_row_to_entity(user.scalar())

    async def search_by_id(self, user_id: UUID | str) -> Optional[User]:
        async with Session() as session:
            query = select(UserSchema).where(UserSchema.id == user_id)
            user = await session.execute(query)

        return user_row_to_entity(user.scalar())
