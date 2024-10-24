from themis_backend.domain.use_cases import DeleteMessage
from themis_backend.external.repositories import PostgreMessageRepository
from themis_backend.presentation.controllers import (
    Controller,
    DeleteMessageController,
)


def delete_message_compose() -> Controller:
    repository = PostgreMessageRepository()
    use_case = DeleteMessage(repository)
    controller = DeleteMessageController(use_case)
    return controller
