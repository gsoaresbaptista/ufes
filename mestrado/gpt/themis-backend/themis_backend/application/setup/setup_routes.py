from starlette.applications import Starlette

from themis_backend.application.routes import (
    clear_chat,
    continue_answer,
    create_user_route,
    delete_message,
    get_last_message,
    get_messages,
    question_route,
    refresh_token_route,
    sign_in_route,
)


# TODO: add openapi and an interface
def setup_routes(app: Starlette) -> None:
    for (route, handler, method) in [
        ('/users/sign-up', create_user_route, 'POST'),
        ('/users/sign-in', sign_in_route, 'POST'),
        ('/users/refresh-token', refresh_token_route, 'POST'),
        ('/messages/question', question_route, 'POST'),
        ('/messages/last', get_last_message, 'GET'),
        ('/messages', get_messages, 'GET'),
        ('/messages/clear-chat', clear_chat, 'DELETE'),
        ('/messages/continue', continue_answer, 'POST'),
        ('/messages/{message_id:str}', delete_message, 'DELETE'),
    ]:
        app.add_route(route, handler, methods=[method])
