from sqlalchemy.ext.asyncio import async_sessionmaker, create_async_engine

from themis_backend.config import DatabaseSettings
from themis_backend.infra.schemas import (  # noqa: F401
    BaseSchema,
    MessageSchema,
    RefreshTokenSchema,
    UserSchema,
)


async def create_tables() -> None:
    async with engine.begin() as conn:
        await conn.run_sync(BaseSchema.metadata.create_all)


engine = create_async_engine(
    'postgresql+asyncpg://{}:{}@{}:{}/{}'.format(
        DatabaseSettings.DATABASE_USER,
        DatabaseSettings.DATABASE_PASSWORD,
        DatabaseSettings.DATABASE_HOST,
        DatabaseSettings.DATABASE_PORT,
        DatabaseSettings.DATABASE_NAME,
    ),
)


Session = async_sessionmaker(engine, expire_on_commit=False)
