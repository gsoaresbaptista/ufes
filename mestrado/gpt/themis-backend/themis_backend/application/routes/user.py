from starlette.requests import Request
from starlette.responses import JSONResponse, Response

from themis_backend.application.adapters import request_adapter
from themis_backend.application.factories import (
    create_user_composer,
    refresh_token_composer,
    sign_in_composer,
)


async def create_user_route(request: Request) -> Response:
    response = await request_adapter(request, create_user_composer())

    return JSONResponse(
        status_code=response.status_code,
        content=response.body,
    )


async def sign_in_route(request: Request) -> Response:
    response = await request_adapter(request, sign_in_composer())

    return JSONResponse(
        status_code=response.status_code,
        content=response.body,
    )


async def refresh_token_route(request: Request) -> Response:
    response = await request_adapter(request, refresh_token_composer())

    return JSONResponse(
        status_code=response.status_code,
        content=response.body,
    )
