from themis_backend.presentation.http import HttpRequest, HttpResponse
from themis_backend.presentation.validators import QuestionValidator

from .controller import Controller


class QuestionController(Controller):
    async def handle(self, http_request: HttpRequest) -> HttpResponse:
        question = http_request.body.get('question', None)
        settings = http_request.body.get('settings', dict())

        validator = QuestionValidator(
            {'question': question, 'settings': settings}
        )
        validator.validate()

        return question, settings, http_request.authorization
