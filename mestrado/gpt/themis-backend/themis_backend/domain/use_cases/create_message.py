from typing import Optional

from themis_backend.domain.entities import Message
from themis_backend.domain.repositories import MessageRepository
from themis_backend.presentation.dtos import CreateMessageDTO


class CreateMessage:
    def __init__(self, repository: MessageRepository) -> None:
        self.__repository = repository

    async def execute(
        self, message_dto: CreateMessageDTO
    ) -> Optional[Message]:
        message = await self.__repository.create(
            user_id=message_dto.user_id,
            question=message_dto.question,
            answer=message_dto.answer,
        )

        return message
