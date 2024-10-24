from typing import Any

from cerberus import Validator

from themis_backend.presentation.http import HTTPUnprocessableEntity


class BaseValidator:
    _query_validator: Validator

    def __init__(self, dto: Any) -> None:
        self.__dto = dto
        self.__query = self._query_validator

    def validate(self) -> None:
        response = self.__query.validate(
            self.__dto
            if isinstance(self.__dto, dict)
            else self.__dto._asdict()
        )
        if not response:
            raise HTTPUnprocessableEntity(errors=self.__query.errors)
