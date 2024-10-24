from cerberus import Validator

from .base import BaseValidator


class SignInValidator(BaseValidator):
    _query_validator = Validator(
        {
            'email': {
                'type': 'string',
                'minlength': 8,
                'maxlength': 255,
                'required': True,
            },
            'password': {
                'type': 'string',
                'minlength': 8,
                'maxlength': 255,
                'required': True,
            },
        }
    )
