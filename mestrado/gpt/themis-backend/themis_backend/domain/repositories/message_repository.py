from abc import ABC, abstractmethod
from typing import Optional
from uuid import UUID

from themis_backend.domain.entities import Message


class MessageRepository(ABC):
    @abstractmethod
    async def create(
        self, user_id: str, question: str, answer: str
    ) -> Optional[Message]:
        ...

    @abstractmethod
    async def search_by_user_id(self, user_id: UUID | str) -> list[Message]:
        ...

    @abstractmethod
    async def get_last_message(self, user_id: UUID | str) -> list[Message]:
        ...

    @abstractmethod
    async def search_by_id(self, message_id: UUID | str, user_id: UUID | str) -> Optional[Message]:
        ...

    @abstractmethod
    async def delete(self, message_id: UUID | str) -> UUID | str:
        ...

    @abstractmethod
    async def delete_all(self, user_id: UUID | str) -> list[UUID | str]:
        ...

    @abstractmethod
    async def update_answer(
        self, user_id: UUID | str, message_id: UUID | str, answer: str
    ) -> list[UUID | str]:
        ...
