from abc import ABC, abstractmethod

from themis_backend.presentation.http import HttpRequest, HttpResponse


class Controller(ABC):
    @abstractmethod
    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        ...
