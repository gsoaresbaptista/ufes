from typing import Optional
from uuid import UUID

from sqlalchemy import delete, select

from themis_backend.domain.entities import RefreshToken
from themis_backend.domain.repositories import RefreshTokenRepository
from themis_backend.infra.database import Session
from themis_backend.infra.schemas import RefreshTokenSchema


def refresh_token_row_to_entity(
    row: RefreshTokenSchema,
) -> Optional[RefreshToken]:
    if row:
        return RefreshToken(
            id=row.id,
            user_id=row.user_id,
            expires_in=row.expires_in,
        )
    return None


class PostgreRefreshTokenRepository(RefreshTokenRepository):
    async def create(self, user_id: UUID | str) -> Optional[RefreshToken]:

        refresh_token = RefreshTokenSchema(user_id=user_id)

        async with Session() as session:
            session.add(refresh_token)
            await session.commit()
            await session.refresh(refresh_token)

        return refresh_token_row_to_entity(refresh_token)

    async def search_by_user_id(
        self, user_id: UUID | str
    ) -> Optional[RefreshToken]:
        async with Session() as session:
            query = select(RefreshTokenSchema).where(
                RefreshTokenSchema.user_id == user_id
            )
            refresh_token = await session.execute(query)

        return refresh_token_row_to_entity(refresh_token.scalar())

    async def delete_all(self, user_id: UUID | str) -> None:
        async with Session() as session:
            query = delete(RefreshTokenSchema).where(
                RefreshTokenSchema.user_id == user_id
            )
            await session.execute(query)
            await session.commit()

    async def search_by_id(
        self, token_id: UUID | str
    ) -> Optional[RefreshToken]:
        async with Session() as session:
            query = select(RefreshTokenSchema).where(
                RefreshTokenSchema.id == token_id
            )
            refresh_token = await session.execute(query)

        return refresh_token_row_to_entity(refresh_token.scalar())
