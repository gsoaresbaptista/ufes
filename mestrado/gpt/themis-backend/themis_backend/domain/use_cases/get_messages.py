from themis_backend.domain.entities import Message
from themis_backend.domain.repositories import MessageRepository
from themis_backend.presentation.dtos import TokenDTO


# TODO: add pagination
class GetMessages:
    def __init__(self, repository: MessageRepository) -> None:
        self.__repository = repository

    async def execute(self, token_dto: TokenDTO) -> list[Message]:
        messages = await self.__repository.search_by_user_id(
            user_id=token_dto.user_id
        )

        return messages
