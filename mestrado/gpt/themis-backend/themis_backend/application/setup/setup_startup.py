import asyncio

from starlette.applications import Starlette

from themis_backend.external.services import (  # noqa: F401
    GGUFModelService,
    RandomModelService,
)
from themis_backend.infra.database import create_tables


async def startup_event_handler() -> None:
    await create_tables()


def setup_startup(app: Starlette) -> None:
    app.add_event_handler('startup', startup_event_handler)
    app.model_lock = asyncio.Lock()
    app.model = GGUFModelService()
