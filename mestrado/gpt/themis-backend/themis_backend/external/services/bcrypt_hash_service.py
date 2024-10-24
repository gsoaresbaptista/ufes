from passlib.context import CryptContext

from themis_backend.domain.services import HashService


class BcryptHashService(HashService):
    def __init__(self) -> None:
        self.__ctx = CryptContext(schemes=['bcrypt'], deprecated='auto')

    def hash(self, password: str) -> str:
        return self.__ctx.hash(password)

    def compare(self, password: str, hashed_password: str) -> bool:
        return self.__ctx.verify(password, hashed_password)
