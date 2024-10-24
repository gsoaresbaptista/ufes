from cerberus import Validator

from .base import BaseValidator


class RefreshTokenValidator(BaseValidator):
    _query_validator = Validator(
        {
            'refresh_token': {
                'type': 'string',
                'empty': False,
                'required': True,
            },
        }
    )
