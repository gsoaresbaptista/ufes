from dataclasses import dataclass
from typing import Any, Optional

from themis_backend.presentation.dtos import TokenDTO


@dataclass
class HttpRequest:
    body: Optional[Any]
    header: dict['str', Any]
    authorization: Optional[TokenDTO] = None


@dataclass
class HttpResponse:
    status_code: int
    body: Any
    authorization: Optional[TokenDTO] = None
