from themis_backend.domain.use_cases import ClearChat
from themis_backend.external.repositories import PostgreMessageRepository
from themis_backend.presentation.controllers import (
    ClearChatController,
    Controller,
)


def clear_chat_compose() -> Controller:
    repository = PostgreMessageRepository()
    use_case = ClearChat(repository)
    controller = ClearChatController(use_case)
    return controller
