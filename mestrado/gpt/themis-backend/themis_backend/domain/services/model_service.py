from abc import ABC, abstractmethod
from typing import Generator


class BufferedGenerator(Generator):
    def __init__(self, default_generator: Generator[str, None, None]):
        self.__generator = default_generator
        self.__buffer = ''

    def send(self, value=None) -> str:
        try:
            text = next(self.__generator)
            self.__buffer += text
            return text
        except StopIteration:
            raise StopIteration

    def throw(self, exc_type, exc_value, traceback):
        pass

    def __next__(self):
        return self.send(None)

    def __iter__(self):
        return self

    def get_text(self) -> str:
        return self.__buffer


class ModelService(ABC):
    @abstractmethod
    async def generate(
        self, question: str, settings: dict[str, float] = dict()
    ) -> BufferedGenerator:
        ...

    @abstractmethod
    async def tokenize(
        self, question: str, settings: dict[str, float] = dict()
    ) -> list[str]:
        ...
