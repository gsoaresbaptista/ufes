from themis_backend.domain.use_cases import CreateUser
from themis_backend.presentation.dtos import CreateUserDTO
from themis_backend.presentation.http import HttpRequest, HttpResponse

from .controller import Controller


class CreateUserController(Controller):
    def __init__(self, use_case: CreateUser) -> None:
        self.__use_case = use_case

    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        name = http_request.body.get('name', None)
        email = http_request.body.get('email', None)
        password = http_request.body.get('password', None)

        dto = CreateUserDTO(name=name, email=email, password=password)
        user = await self.__use_case.execute(dto)

        return HttpResponse(
            status_code=201, body={'data': {'user': user.to_dict()}}
        )
