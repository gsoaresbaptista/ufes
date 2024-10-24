from typing import Any


class HTTPError(Exception):
    def __init__(self, status_code: int, message: str) -> None:
        super().__init__(message)
        self.message = message
        self.status_code = status_code

    def to_dict(self) -> dict[str, Any]:
        return {'status_code': self.status_code, 'message': self.message}


class HTTPUnprocessableEntity(HTTPError):
    def __init__(self, errors: dict) -> None:
        super().__init__(
            422, 'Correct request structure, hindered by semantic errors.'
        )
        self.errors = errors

    def to_dict(self) -> dict[str, Any]:
        dict_data = super().to_dict()
        dict_data.update({'errors': self.errors})
        return dict_data


class HTTPUnauthorized(HTTPError):
    def __init__(self, message: str = 'Unauthorized credentials') -> None:
        super().__init__(401, message)


class HTTPBadRequest(HTTPError):
    def __init__(self, message: str = 'Empty or Missing Request Body') -> None:
        super().__init__(400, message)


class HTTPForbidden(HTTPError):
    def __init__(self, message: str = "You don't have permission") -> None:
        super().__init__(403, message)


class HTTPNotFound(HTTPError):
    def __init__(self, message: str = 'Resource not found') -> None:
        super().__init__(404, message)


class HTTPConflict(HTTPError):
    def __init__(self, message: str = 'Conflict in the resource') -> None:
        super().__init__(409, message)


class UserNotFound(HTTPNotFound):
    def __init__(self, message: str = 'User not found') -> None:
        super().__init__(message)


class UserAlreadyExists(HTTPConflict):
    def __init__(self, email: str) -> None:
        super().__init__(f'The email address {email} is already in use.')
        self.email = email

    def to_dict(self) -> dict[str, Any]:
        dict_data = super().to_dict()
        dict_data.update({'conflict_key': 'email', 'email': self.email})
        return dict_data


class IncorrectPassword(HTTPUnauthorized):
    def __init__(self, message: str = 'Incorrect password') -> None:
        super().__init__(message)


class EmptyQuestionNotAllowed(HTTPBadRequest):
    def __init__(self, message: str = 'Empty question is not allowed') -> None:
        super().__init__(message)
