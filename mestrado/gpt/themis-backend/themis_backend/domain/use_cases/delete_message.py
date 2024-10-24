from uuid import UUID

from themis_backend.domain.repositories import MessageRepository
from themis_backend.presentation.dtos import DeleteMessageDTO


class DeleteMessage:
    def __init__(self, repository: MessageRepository) -> None:
        self.__repository = repository

    async def execute(self, delete_dto: DeleteMessageDTO) -> UUID | str:
        return await self.__repository.delete(
            user_id=delete_dto.user_id, message_id=delete_dto.message_id
        )
