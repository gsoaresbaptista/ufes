from cerberus import Validator

from .base import BaseValidator
from themis_backend.config import ModelSettings


class QuestionValidator(BaseValidator):
    _query_validator = Validator(
        {
            'question': {
                'type': 'string',
                'empty': False,
                'maxlength': int(ModelSettings.CONTEXT_LENGTH/2),
                'required': True,
            },
            'settings': {
                'type': 'dict',
                'required': False,
                'schema': {
                    'temperature': {
                        'type': 'float',
                        'min': 0,
                        'max': 1,
                        'required': False,
                    },
                    'last_n_tokens': {
                        'type': 'integer',
                        'min': 1,
                        'required': False,
                    },
                    'repetition_penalty': {
                        'type': 'float',
                        'required': False,
                    },
                },
            },
        }
    )
