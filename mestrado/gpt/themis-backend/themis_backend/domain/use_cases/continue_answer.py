from typing import Optional

from themis_backend.domain.entities import Message
from themis_backend.domain.repositories import MessageRepository
from themis_backend.presentation.dtos import ContinueAnswerDTO


class ContinueAnswer:
    def __init__(self, repository: MessageRepository) -> None:
        self.__repository = repository

    async def execute(
        self, continue_dto: ContinueAnswerDTO
    ) -> Optional[Message]:
        message = await self.__repository.search_by_id(
            message_id=continue_dto.message_id, user_id=continue_dto.user_id
        )

        return message
