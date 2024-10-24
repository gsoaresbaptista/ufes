from uuid import UUID

from themis_backend.domain.repositories import MessageRepository
from themis_backend.presentation.dtos import ClearChatDTO


class ClearChat:
    def __init__(self, repository: MessageRepository) -> None:
        self.__repository = repository

    async def execute(self, clear_dto: ClearChatDTO) -> list[UUID | str]:
        return await self.__repository.delete_all(user_id=clear_dto.user_id)
