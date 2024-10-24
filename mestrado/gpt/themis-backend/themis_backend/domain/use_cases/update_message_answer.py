from typing import Optional

from themis_backend.domain.entities import Message
from themis_backend.domain.repositories import MessageRepository
from themis_backend.presentation.dtos import UpdateMessageAnswerDTO


class UpdateMessageAnswer:
    def __init__(self, repository: MessageRepository) -> None:
        self.__repository = repository

    async def execute(
        self, update_dto: UpdateMessageAnswerDTO
    ) -> Optional[Message]:
        message = await self.__repository.update_answer(
            user_id=update_dto.user_id,
            message_id=update_dto.message_id,
            answer=update_dto.answer,
        )

        return message
