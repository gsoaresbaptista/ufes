from themis_backend.domain.use_cases import GetLastMessage
from themis_backend.external.repositories import PostgreMessageRepository
from themis_backend.presentation.controllers import (
    Controller,
    GetLastMessageController,
)


def get_last_messages_compose() -> Controller:
    repository = PostgreMessageRepository()
    use_case = GetLastMessage(repository)
    controller = GetLastMessageController(use_case)
    return controller
