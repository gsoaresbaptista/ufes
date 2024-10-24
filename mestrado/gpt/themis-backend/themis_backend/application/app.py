from starlette.applications import Starlette
from starlette.middleware import Middleware
from starlette.middleware.cors import CORSMiddleware

from themis_backend.application.setup import (
    setup_exceptions,
    setup_routes,
    setup_startup,
)

app = Starlette(
    debug=True,
    middleware=[
        Middleware(
            CORSMiddleware,
            allow_origins=['*'],
            allow_methods=['*'],
            allow_headers=['*'],
            allow_credentials=True,
        )
    ],
)

setup_routes(app)
setup_exceptions(app)
setup_startup(app)
