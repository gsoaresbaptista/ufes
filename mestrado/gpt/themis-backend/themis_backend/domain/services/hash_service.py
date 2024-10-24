from abc import ABC, abstractmethod


class HashService(ABC):
    @abstractmethod
    def hash(self, password: str) -> str:
        ...

    @abstractmethod
    def compare(self, password: str, hashed_password: str) -> bool:
        ...
