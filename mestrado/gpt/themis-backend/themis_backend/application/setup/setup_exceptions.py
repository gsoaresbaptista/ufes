from starlette.applications import Starlette
from starlette.requests import Request
from starlette.responses import JSONResponse

from themis_backend.presentation.http import HTTPError


async def default_exception(request: Request, exception: Exception):
    return JSONResponse(
        content={
            'status_code': 500,
            'error': type(exception),
        },
        status_code=500,
    )


async def http_exception(request: Request, exception: HTTPError):
    return JSONResponse(
        content=exception.to_dict(), status_code=exception.status_code
    )


def setup_exceptions(app: Starlette) -> None:
    app.add_exception_handler(HTTPError, http_exception)
    app.add_exception_handler(500, default_exception)
