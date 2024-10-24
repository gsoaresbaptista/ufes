from cerberus import Validator

from .base import BaseValidator


class CreateUserValidator(BaseValidator):
    _query_validator = Validator(
        {
            'name': {
                'type': 'string',
                'empty': False,
                'maxlength': 80,
                'required': True,
            },
            'email': {
                'type': 'string',
                'minlength': 8,
                'maxlength': 255,
                'required': True,
                'regex': '^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$',
            },
            'password': {
                'type': 'string',
                'minlength': 8,
                'maxlength': 255,
                'required': True,
                'regex': '^(?=.*[A-Z])(?=.*[\W_]).*$',
            },
        }
    )
