from themis_backend.domain.use_cases import ContinueAnswer
from themis_backend.external.repositories import PostgreMessageRepository
from themis_backend.presentation.controllers import (
    ContinueAnswerController,
    Controller,
)


def continue_answer_compose() -> Controller:
    repository = PostgreMessageRepository()
    use_case = ContinueAnswer(repository)
    controller = ContinueAnswerController(use_case)
    return controller
