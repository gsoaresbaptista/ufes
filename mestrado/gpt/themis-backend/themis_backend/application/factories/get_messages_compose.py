from themis_backend.domain.use_cases import GetMessages
from themis_backend.external.repositories import PostgreMessageRepository
from themis_backend.presentation.controllers import (
    Controller,
    GetMessagesController,
)


def get_messages_compose() -> Controller:
    repository = PostgreMessageRepository()
    use_case = GetMessages(repository)
    controller = GetMessagesController(use_case)
    return controller
