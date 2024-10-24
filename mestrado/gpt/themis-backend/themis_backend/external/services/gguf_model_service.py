from ctransformers import AutoConfig, AutoModelForCausalLM, Config

from themis_backend.config import ModelSettings
from themis_backend.domain.services import BufferedGenerator, ModelService


class GGUFModelService(ModelService):
    def __init__(self) -> None:
        self.__model = AutoModelForCausalLM.from_pretrained(
            ModelSettings.FOLDER_PATH,
            model_file=ModelSettings.FILE_PATH,
            gpu_layers=ModelSettings.GPU_LAYERS,
            add_eos_token=True,
            config=AutoConfig(
                Config(
                    context_length=ModelSettings.CONTEXT_LENGTH,
                    gpu_layers=ModelSettings.GPU_LAYERS,
                    temperature=0.8,
                    top_p=0.6,
                    repetition_penalty=1.2,
                ),
            ),
        )
        self.__token_to_str = self.__model.ctransformers_llm_detokenize

    async def generate(
        self, question: str, settings: dict[str, float] = dict()
    ) -> BufferedGenerator:

        settings['max_new_tokens'] = ModelSettings.MAX_NEW_TOKENS

        return BufferedGenerator(
            self.__model(question, stream=True, **settings)
        )

    async def tokenize(
        self, question: str, settings: dict[str, float] = dict()
    ) -> list[str]:
        tokens = []
        for token in self.__model.tokenize(question):
            tokens.append((await self.__token_to_str(token)).decode('utf-8'))
        return tokens
