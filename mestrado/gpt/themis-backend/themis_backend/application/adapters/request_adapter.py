import json

from starlette.requests import Request

from themis_backend.presentation.controllers import Controller
from themis_backend.presentation.http import HttpRequest, HttpResponse


async def request_adapter(
    request: Request,
    controller: Controller,
    middlewares: list[Controller] = list(),
) -> HttpResponse:
    body = {}

    if request.method in ['POST', 'PUT']:
        try:
            body = await request.json()
        except:  # noqa: E722
            pass

    if isinstance(body, str):
        body = json.loads(body)

    body.update(request.path_params)
    body.update(request.query_params)

    http_request = HttpRequest(body=body, header=request.headers)

    for middleware in middlewares:
        http_request = await middleware.handle(http_request)

    http_response = await controller.handle(http_request)

    return http_response
